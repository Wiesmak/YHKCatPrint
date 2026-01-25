/*
 * Copyright (c) 2026. Umamusume Polska
 */

plugins {
    base
    `jacoco-report-aggregation`
}

dependencies {
    jacocoAggregation(project(":app"))
    jacocoAggregation(project(":bluetooth"))
}

reporting {
    reports {
        val testCodeCoverageReport by creating(JacocoCoverageReport::class) {
            testSuiteName = "test"
        }
    }
}

tasks.named<JacocoReport>("testCodeCoverageReport") {
    reports {
        xml.required.set(true)
        html.required.set(true)
    }
}